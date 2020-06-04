#include "voxel.h"


void Voxel::init(){
  flags = 0;
}

bool Voxel::is_active(){
  return (flags & VOXELFLAG_ACTIVE) != 0;
}

void Voxel::set_active(bool active){
  if(active){
    flags |= VOXELFLAG_ACTIVE;  
  }else{
    flags &= ~VOXELFLAG_ACTIVE;
  }
  
}

Voxel::VoxelType Voxel::get_type(){
  return static_cast<VoxelType>(flags & VOXELFLAG_TYPE);
}

void Voxel::set_type(Voxel::VoxelType type){
  flags &= (~VOXELFLAG_TYPE);
  flags |= (uint32_t) type;
}
