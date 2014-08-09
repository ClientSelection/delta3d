
attribute vec4 tangentAttrib;

varying vec3 vLightDir;
varying vec3 vNormal;
varying vec3 vPos;
varying vec3 vCamera;
varying vec2 vReflectTexCoord;
varying vec3 vViewDir;

uniform mat4 inverseViewMatrix;

void sphereMap(vec3, vec3, out vec2);
float calculateDistance(mat4, vec4);

void main()
{   
   // Pass the texture coordinate on through.
   gl_TexCoord[0] = gl_MultiTexCoord0;
   gl_FogFragCoord = gl_FogCoord;

   // Moves the position, normal, and light direction into world space   
   vPos = (inverseViewMatrix * gl_ModelViewMatrix * gl_Vertex).xyz;
   mat3 inverseView3x3 = mat3(inverseViewMatrix[0].xyz, inverseViewMatrix[1].xyz, inverseViewMatrix[2].xyz);

   vCamera = inverseViewMatrix[3].xyz;
   
   vNormal = normalize(gl_NormalMatrix * gl_Normal);
//   vNormal = inverseView3x3 * gl_NormalMatrix * gl_Normal;

   vec3 lightDir = normalize(gl_LightSource[0].position.xyz);
//   vLightDir = normalize(inverseView3x3 * gl_LightSource[0].position.xyz);
   
   
   
   
   vec3 tangent   = normalize(gl_NormalMatrix * tangentAttrib.xyz);
   vec3 bitangent = cross(vNormal, tangent);

   // gram-schmidt tangent orthonormalization
   float tangentDOTnormal = dot(tangent, vNormal);
   vec3 tempNormal = vNormal * tangentDOTnormal;
   tangent -= tempNormal;   
 
   float bitangentDOTnormal = dot(bitangent, vNormal);
   tempNormal = vNormal * bitangentDOTnormal;
   
   float bitangentDOTtangent = dot(bitangent, tangent);
   vec3 tempTan = tangent * bitangentDOTtangent;
   vec3 tempBi = tempNormal - tempTan;
   bitangent -= tempBi;
   
   vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
   
   vec3 temp;
   temp.x = dot(lightDir, tangent);
   temp.y = dot(lightDir, bitangent);
   temp.z = dot(lightDir, vNormal);
   vLightDir = normalize(temp);
   
   vec3 eyeDir = -vec3(gl_ModelViewMatrix * gl_Vertex);

   temp.x = dot(eyeDir, tangent);
   temp.y = dot(eyeDir, bitangent);
   temp.z = dot(eyeDir, vNormal);
   vViewDir = normalize(temp);
   

   // Compute the reflection map UV coordinate
   sphereMap(vViewDir, vNormal, vReflectTexCoord);

   // Compute the final vertex position in clip space.
   gl_Position = ftransform();

}