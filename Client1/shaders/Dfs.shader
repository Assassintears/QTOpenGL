#version 450 core
in vec4 Color;
out vec4 FragColor;
//uniform vec3 objectColor;//物体反射的颜色
//uniform vec3 lightColor;//光源颜色
void main() 
{
//    float ambientStrength = 0.1;
//    vec3 ambient = ambientStrength * lightColor;
//    vec3 result = ambient * objectColor;
   //vec3 p = normalize(vec3(Color.x, Color.y, Color.z));
//   FragColor = vec4(result, 1.0);
//    FragColor = vec4(vec3(gl_FragCoord.z), 1.0);//深度测试可视化
//    if (gl_FragCoord.x < 100)
//    {
//        FragColor = vec4(1, 0, 0, 0);
//    }
//    else
//    {
        FragColor = Color;
//    }

}
