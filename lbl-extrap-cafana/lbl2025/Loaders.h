
#ifndef LBL2025_LOADERS_H
#define LBL2025_LOADERS_H

#include "CAFAna/Core/Loaders.h"

namespace lbl2025
{
	class Loaders
	{
        public:
			explicit Loaders(const ana::RecoType & recoType);

			ana::Loaders * GetLoaders(const ana::FluxType & flux);

			void Go();

        private:
			std::unique_ptr<ana::Loaders> fFHC;
			std::unique_ptr<ana::Loaders> fRHC;
	};

}

#endif //LBL2025_LOADERS_H
