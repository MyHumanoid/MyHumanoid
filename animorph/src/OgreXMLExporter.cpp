#include "../include/animorph/OgreXMLExporter.h"
#include "../include/animorph/xmlParser.h"
using namespace std;
using namespace Animorph;

bool OgreXMLExporter::exportFile (const string& filename)
{

  //TextureVector  &texturevector  (mesh.getTextureVectorRef ());
  MaterialVector &materialvector (mesh.getMaterialVectorRef ());

  string temp,temp2;
  string file_ending_cut(cutFileEnding (filename, ".obj"));

  string bname = file_ending_cut.substr (filename.find_last_of (PATH_SEPARATOR)+1, filename.size ());
  int index = bname.find_last_of("/");
  temp = bname.substr(index+1);

  XMLNode xMainNode,xNodeSubmeshes,xNode_mesh;
  xMainNode = XMLNode::createXMLTopNode("",TRUE);
  xNode_mesh = xMainNode.addChild("mesh");
  xNodeSubmeshes = xNode_mesh.addChild("submeshes");

  //subdivide by materials:
  //int mv_size = materialvector.size();
  for (uint32_t m = 0 ;m<materialvector.size();m++){
    bool found = false;
    for (uint32_t m1= 0; m1<m;m1++){

      //already rendered this material???
      if(materialvector[m1].getName() == materialvector[m].getName()){
       found = true;
       break;
      }
    }
    if (found == true)
      continue;
    InsertASubmeshNode(&xNodeSubmeshes,NULL,m);
  }
  return (eXMLErrorNone == xMainNode.writeToFile((file_ending_cut+".mesh.xml").c_str()));
}
void OgreXMLExporter::InsertASubmeshNode(XMLNode *xNodeSubmeshes,XMLNode *Node, int index_material)
{
  MaterialVector &materialvector (mesh.getMaterialVectorRef ());
  TextureVector  &texturevector  (mesh.getTextureVectorRef ());
  FaceVector     &facevector     (mesh.getFaceVectorRef ());
  VertexVector   &vertexvector   (mesh.getVertexVectorRef ());

  XMLNode xNodeSubmesh;
  XMLNode xNode_faces;
  XMLNode xNode_face,xNode_geometry,xNode_vertexbuffer,xNode_vertexbuffer_uv,xNode_vertex;;
  unsigned int i , j ;
  int vx_counter = -1, vx_counter_max = -1;
  int faces_counter = 0;

  xNodeSubmesh = xNodeSubmeshes->addChild("submesh");
  xNode_faces = xNodeSubmesh.addChild("faces");
  xNode_geometry = xNodeSubmesh.addChild("geometry");

  xNode_vertexbuffer = xNode_geometry.addChild("vertexbuffer");
  xNode_vertexbuffer.addAttribute("positions","true");
  xNode_vertexbuffer.addAttribute("normals","true");

  xNode_vertexbuffer_uv = xNode_geometry.addChild("vertexbuffer");
  xNode_vertexbuffer_uv.addAttribute("texture_coord_dimensions_0","2");
  xNode_vertexbuffer_uv.addAttribute("texture_coords","1");

  xNodeSubmesh.addAttribute("material",materialvector[index_material].getName ().c_str());
  xNodeSubmesh.addAttribute("usesharedvertices","false");
  xNodeSubmesh.addAttribute("use32bitindexes","false");
  xNodeSubmesh.addAttribute("operationtype","triangle_list");
  //unsigned int facevectorsize = facevector.size ();

  for ( i = 0; i < facevector.size (); i++)
  {
    const Face &face(facevector[i]);

    if(materialvector[face.getMaterialIndex()].getName() != materialvector[index_material].getName())
      continue;



    xNode_face = xNode_faces.addChild("face");
    int triangle = 0;
    unsigned int casize = face.getSize();
    if (casize >4)
      casize++;
    if (casize <3)
      casize++;

    faces_counter++;
    for (j = 0; j < face.getSize(); j++)
    {
      int vertex_number = face.getVertexAtIndex(j);
      //cout << texture_number << endl;

      Vertex &vertex = vertexvector[vertex_number];
      Vector3f vector = vertex.co * tm;
      Vector3f vector_no = vertex.no * tm;
      vx_counter++;

      std::ostringstream out_stream;
      out_stream << vx_counter;
      switch(j%3){
        case 0:
          xNode_face.addAttribute("v1",out_stream.str().c_str());
          break;
        case 1:
          xNode_face.addAttribute("v2",out_stream.str().c_str());
          break;
        case 2:
          xNode_face.addAttribute("v3",out_stream.str().c_str());
          break;
       }

      TextureFace &texture_face = texturevector[i];
      Vector2f &uv = texture_face[j];

      if(vx_counter>vx_counter_max){
        vx_counter_max = vx_counter;

        XMLNode xNode_normal,xNode_position,xNode_texcoord;
        xNode_vertex = xNode_vertexbuffer.addChild("vertex");
        xNode_position = xNode_vertex.addChild("position");
        xNode_normal = xNode_vertex.addChild("normal");

        std::ostringstream x,y,z,x1,y1,z1,x2,y2;
        x << vector.x;
        xNode_position.addAttribute("x",x.str().c_str());
        y << vector.y;
        xNode_position.addAttribute("y",y.str().c_str());
        z << vector.z;
        xNode_position.addAttribute("z",z.str().c_str());



        x1 << vector_no.x;
        xNode_normal.addAttribute("x",x1.str().c_str());
        y1 << vector_no.y;
        xNode_normal.addAttribute("y",y1.str().c_str());
        z1 << vector_no.z;
        xNode_normal.addAttribute("z",z1.str().c_str());

        xNode_vertex = xNode_vertexbuffer_uv.addChild("vertex");
        xNode_texcoord = xNode_vertex.addChild("texcoord");


        x2 << uv.x;
        xNode_texcoord.addAttribute("u",x2.str().c_str());

        y2 << uv.y;
        xNode_texcoord.addAttribute("v",y2.str().c_str());

      }
      triangle ++;
      if((j%3)==2 && ( (j+1) != face.getSize()) && triangle == 3){
         j -= 2;
         vx_counter -= 2;
         xNode_face = xNode_faces.addChild("face");
         triangle = 0;
         faces_counter++;
      }
    }

  }
  std::ostringstream out_stream,out_stream2;
  out_stream << faces_counter;

  xNode_faces.addAttribute("count",out_stream.str().c_str());

  out_stream2 <<  (vx_counter_max + 1);
  xNode_geometry.addAttribute("vertexcount",out_stream2.str().c_str());

  if (faces_counter == 0)
   xNodeSubmesh.deleteNodeContent(0);
}
