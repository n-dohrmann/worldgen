
#ifndef INCLUDE_PATHS
#define INCLUDE_PATHS

// when building in the CI pipeline, we won't
// have access to some external include files, so they'll
// be found separately. S = single, D = double
#ifdef CI_BUILD
#define STB_IMAGE_PATH_S "/tmp/stb_image.h"
#define STB_IMAGE_PATH_D "/tmp/stb_image.h"
#define STB_IMAGE_WRITE_PATH_S "/tmp/stb_image_write.h"
#define STB_IMAGE_WRITE_PATH_D "/tmp/stb_image_write.h"
#else // CI_BUILD
#define STB_IMAGE_PATH_S "../stb/stb_image.h"
#define STB_IMAGE_PATH_D "../../stb/stb_image.h"
#define STB_IMAGE_WRITE_PATH_S "../stb/stb_image_write.h"
#define STB_IMAGE_WRITE_PATH_D "../../stb/stb_image_write.h"
#endif // CI_BUILD

#endif // INCLUDE_PATHS
