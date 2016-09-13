#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};  

struct Light {
    vec3 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
out vec4 color;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // Ambient
    vec4 ambient = light.ambient * vec4(texture(material.diffuse, TexCoords));
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(0.0, dot(norm, lightDir));
    vec4 diffuse = light.diffuse * diff * vec4(texture(material.diffuse, TexCoords));  
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir); 
    float spec = pow(max(0.0, dot(norm, halfwayDir)), material.shininess);
    vec4 specular = light.specular * spec * vec4(texture(material.specular, TexCoords));
        
    color = vec4(ambient + diffuse + specular);
} 