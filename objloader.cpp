#include "objloader.h"
#include "tiny_obj_loader.hpp"
#include "glm/glm.hpp"


/*	Creates GraphicsObjects from an Obj file and stores them in a vector
 * 	@path Path to the Obj file
 * 
 */
std::vector<GraphicsObject> ObjLoader::loadObj ( const std::string& objpath, const std::string& mtlpath )
{
  //tiny obj structs for storing Vert, Indic, color...
  std::vector <tinyobj::shape_t> shapes;
  std::vector <tinyobj::material_t> mats;
  
  std::vector<GraphicsObject> retVec;
  
  std::string retString = tinyobj::LoadObj(shapes,mats, objpath.c_str(), mtlpath.c_str());
  //error handling
  if(!retString.empty())
  {
    std::cerr << "Sth IsWrong with your obj File: " << retString << std::endl;
    exit(1);
  }
  
  for(int i = 0; i < shapes.size(); i++)
  {
      glm::vec3 diff = glm::vec3(mats[shapes[i].mesh.material_ids[0]].diffuse[0], mats[shapes[i].mesh.material_ids[0]].diffuse[1], mats[shapes[i].mesh.material_ids[0]].diffuse[2]);
      glm::vec3 spec = glm::vec3(mats[shapes[i].mesh.material_ids[0]].specular[0], mats[shapes[i].mesh.material_ids[0]].specular[1],mats[shapes[i].mesh.material_ids[0]].specular[2]);
      std::string texname = mats[shapes[i].mesh.material_ids[0]].diffuse_texname;
      GLfloat shiny = mats[shapes[i].mesh.material_ids[0]].shininess;
      //push back to the vector     
      std::cout << texname << std::endl;
      retVec.push_back(GraphicsObject(shapes[i].mesh.positions, 
				      shapes[i].mesh.indices ,
				      diff, 
				      spec,
				      shapes[i].mesh.texcoords ,
				      shiny, 
				      shapes[i].name,
				      texname
 				    ));
  }   
  return retVec;
}
