#include <ot/simpleot.h>
#include <ot/OTconstants.h>
#include <ENCRYPTO_utils/channel.h>
#include <ENCRYPTO_utils/thread.h>
#include <ENCRYPTO_utils/connection.h>
#include <ENCRYPTO_utils/sndthread.h>
#include <ENCRYPTO_utils/rcvthread.h>
#include <ENCRYPTO_utils/cbitvector.h>
#include <ENCRYPTO_utils/socket.h>
#include <iostream>

static const char* m_cConstSeed[2] = {"437398417012387813714564100", "15657566154164561"};

int
main(int argc, char** argv)
{
	std::string address = "127.0.0.1";
	uint16_t port = 7766;
	uint32_t nsndvals = 2;
	uint32_t m_nSecParam = 128;
	auto m_eFType = ECC_FIELD;

	crypto *m_cCrypt = new crypto(m_nSecParam, (uint8_t*)m_cConstSeed[0]);
  CLock *glock = new CLock();

	//Server listen
	std::unique_ptr<CSocket> m_Socket = Listen(address, port);
	if (!m_Socket) {
		std::cerr << "Listen failed on " << address << ":" << port << "\n";
		std::exit(1);
	}

	auto sndthread = new SndThread(m_Socket.get(), glock);
	auto rcvthread = new RcvThread(m_Socket.get(), glock);

	rcvthread->Start();
	sndthread->Start();

	int64_t m_nBaseOTs = 1000;
	auto m_cBaseOT = new SimpleOT(m_cCrypt, ECC_FIELD);
	channel* chan = new channel(OT_ADMIN_CHANNEL, rcvthread, sndthread);
	uint8_t* pBuf = (uint8_t*) malloc(m_cCrypt->get_hash_bytes() * m_nBaseOTs * nsndvals);

#ifdef OTTiming
	timespec np_begin, np_end;
	clock_gettime(CLOCK_MONOTONIC, &np_begin);
#endif

	m_cBaseOT->Sender(nsndvals, m_nBaseOTs, chan, pBuf);
	std::cout << "endddd" << std::endl;

#ifdef OTTiming
	clock_gettime(CLOCK_MONOTONIC, &np_end);
	printf("Time for performing the base-OTs: %f seconds\n", getMillies(np_begin, np_end));
#endif

	free(pBuf);
	chan->synchronize_end();
	delete(chan);
	delete crypt;
  delete glock;
}