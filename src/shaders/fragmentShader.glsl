#version 330 core

out vec4 FragColor;

in vec3 col;

void main()
{
    FragColor = vec4(col, 1.0f);
    /* 
    The bottom left side of triangle is black because 
    in the 3rd quadrant of the window x and y values are negative, 
    so only in 1, 2 or 4 quadrant the value of color is not 0.
    I guess FragColor values are just clamped between 0 and 1.
    */
} 