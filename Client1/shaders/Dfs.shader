#version 450 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;//camera Position in camera coordinates
    // vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Material material;
uniform Light light;

in vec3 pos_in_cam;
in vec3 normal_in_cam;
in vec4 Color;

void main() 
{
    
    vec3 ambient = light.ambient * material.ambient;
  	
    vec3 norm = normalize(normal_in_cam);
    vec3 lightDir = normalize(light.position - pos_in_cam);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    vec3 viewDir = normalize(-pos_in_cam);
    vec3 reflectDir = reflect(lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(lightDir, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
        
    vec3 result = ambient + diffuse + specular;
    // vec3 result = specular;
    FragColor = vec4(result, 1.0);
    // FragColor = Color;
}
