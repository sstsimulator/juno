
#ifndef _H_SST_JUNO_GEN_RAND_EVENT
#define _H_SST_JUNO_GEN_RAND_EVENT

#include <sst/core/event.h>

namespace SST {
namespace Juno {

class JunoGenerateRandEvent : public SST::Event {

public:
	JunoGenerateRandEvent() : SST::Event() {}
	JunoGenerateRandEvent( const int evID ):
		SST::Event(), id(evID) {}
	~JunoGenerateRandEvent() {}

	int getID() const { return id; }

	void serialize_order( SST::Core::Serialization::serializer &ser) override {
		Event::serialize_order(ser);
		ser & id;
	}

	ImplementSerializable(SST::Juno::JunoGenerateRandEvent);
protected:
	int id;
};

}
}

#endif
