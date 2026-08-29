
// DESCRIPTION:
//  Sky rendering. The DOOM sky is a texture map like any
//  wall, wrapping around. A 1024 columns equal 360 degrees.
//  The default sky map is 256 columns and repeats 4 times
//  on a 320 screen?
//-----------------------------------------------------------------------------

// Needed for FRACUNIT.
#include "Miscellaneous/fixed.hpp"

// Needed for Flat retrieval.
#include "Renderer/data.hpp"

#ifdef __GNUG__
#pragma implementation "Renderer/sky.hpp"
#endif
#include "Renderer/sky.hpp"

// sky mapping
int skyflatnum;
int skytexture;
int skytexturemid;

// R_InitSkyMap
// Called whenever the view size changes.
void R_InitSkyMap(void)
{
  // skyflatnum = R_FlatNumForName ( SKYFLATNAME );
  skytexturemid = 100 * FRACUNIT;
}
