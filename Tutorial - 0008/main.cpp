/* -----------------------------------------------------
This source code is public domain ( CC0 )
The code is provided as-is without limitations, requirements and responsibilities.
Creators and contributors to this source code are provided as a token of appreciation
and no one associated with this source code can be held responsible for any possible
damages or losses of any kind.

Original file creator:  Niko Kauppi (Code maintenance)
Contributors:
----------------------------------------------------- */

#include "Renderer.h"

int main()
{
	Renderer r;

	r.OpenWindow( 800, 600, "Vulkan API Tutorial 8" );

	while( r.Run() ) {

	}

	return 0;
}
