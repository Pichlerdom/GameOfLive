#ifndef _VOXEL_H_
#define _VOXEL_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define VOXEL_D_TYPE uint8_t
#define VOXEL_D_TYPE_LENGTH 8

class Voxel{
public:
  enum VoxelType{
		 VOXELTYPE_DEFAULT = 0,
		 VOXELTYPE_BLUE,
		 VOXELTYPE_RED,
		 VOXELTYPE_TEST,
	       
		 VOXELTYPE_NUMTYPES,
  };
  
  void init();
  bool is_active();
  void set_active(bool active);
  
  VoxelType get_type();
  void set_type(VoxelType type);
public:
  enum VoxelFlag{
		 VOXELFLAG_ACTIVE = (1<<(VOXEL_D_TYPE_LENGTH-1)),
		 VOXELFLAG_TYPE = 64,
  };
  VOXEL_D_TYPE flags = 0;
};

#endif
