#pragma once

#include <string>
#include <vector>
#include <utility>

namespace Sourcehold {
	namespace Parsers {
		/**
		 * A section is a self-contained XML structure.
		 */
		struct HlpSection {
			std::wstring name;
		};

		bool LoadStrongholdHlp();
		void UnloadStrongholdHlp();
	}
}
