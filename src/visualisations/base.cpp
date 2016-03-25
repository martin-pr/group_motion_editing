#include "base.h"

namespace visualisations {

unsigned base::uid() {
	static unsigned s_uid = 0;

	return s_uid++;
}

}
