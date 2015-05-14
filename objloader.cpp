#include "objloader.h"
#include "tiny_obj_loader.hpp"

/*	Creates GraphicsObjects from an Obj file and stores them in a vector
 * 	@path Path to the Obj file
 * 
 */
std::vector<GraphicsObject> ObjLoader::loadObj ( const std::string& path )
{
  //tiny obj structs for storing Vert, Indic, color...
  std::vector <tinyobj::shape_t> shapes;
  std::vector <tinyobj::material_t> mats;
  
  std::vector<GraphicsObject> retVec;
  
  std::string retString = tinyobj::LoadObj(shapes,mats, path.c_str(), NULL);
  //error handling
  if(!retString.empty())
  {
    std::cerr << "SthIsWrong with your obj File" << retString << std::endl;
    exit(1);
  }
  
  for(int i = 0; i < shapes.size(); i++)
  {
    std::vector<GLfloat> color;
    if(mats.empty())
    {
      //when the material is empty just make everything white
      color = std::vector<GLfloat>(shapes[i].mesh.positions.size()/3, 1.0);
    }
    else
    {
      for (int j = 0; j < shapes[i].mesh.indices.size(); j++)
      {
	//run through all the indicies and assign the color which is defined in the material_ids.diffuse to them 
	if (mats.size() > 0 && shapes[i].mesh.material_ids.size() > 0)
	{
	  if(shapes[i].mesh.material_ids.size() > j)
	  {
	    color.push_back(mats[shapes[i].mesh.material_ids[j]].diffuse[0]);
	    color.push_back(mats[shapes[i].mesh.material_ids[j]].diffuse[1]);
	    color.push_back(mats[shapes[i].mesh.material_ids[j]].diffuse[2]);
	  }
	  else //bug fix for the last mats
	  {
	    color.push_back(mats[shapes[i].mesh.material_ids[0]].diffuse[0]);
	    color.push_back(mats[shapes[i].mesh.material_ids[0]].diffuse[1]);
	    color.push_back(mats[shapes[i].mesh.material_ids[0]].diffuse[2]);
	  }
	    
	  }
      }
      //push back to the vector     
      retVec.push_back(GraphicsObject(shapes[i].mesh.positions, color, shapes[i].mesh.indices, shapes[i].name));
    }
  }   
  return retVec;
}
