
#ifndef _H_SST_JUNO_GEN_RAND_RESP_EVENT
#define _H_SST_JUNO_GEN_RAND_RESP_EVENT

#include <sst/core/event.h>

namespace SST {
namespace Juno {

class JunoGenerateRandRespEvent : public SST::Event {

public:
	JunoGenerateRandRespEvent() : SST::Event() {}
	JunoGenerateRandRespEvent( const int evID, const int64_t rand):
		SST::Event(), id(evID), randNum(rand) {}
	~JunoGenerateRandRespEvent() {}

	int getID() const { return id; }
	int64_t getRand() const { return randNum; }

	void serialize_order( SST::Core::Serialization::serializer &ser) override {
		Event::serialize_order(ser);
		ser & id;
		ser & randNum;
	}

	ImplementSerializable(SST::Juno::JunoGenerateRandRespEvent);
protected:
	int id;
	int64_t randNum;
};

}
}

#endif
