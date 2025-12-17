#pragma once

//#define BREACH

#define LUKE2 // hydro
//#define AXADMIN // need to enable LUKE2
#define GAMELORD // need to enable LUKE2
//#define PANDA
//#define d4ftBrand
//#define NotShxdowBrand

 








// no need to change this, it is just a flag to avoid conflicts with other libraries
#ifdef NotShxdowBrand
#define IMGUI_DEFINE_MATH_OPERATORS
#else
#endif