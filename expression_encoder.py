from PIL import Image
import json
import os
## take a directory of expression images and produce a C++ file with enum-referenceable RGB data

#absolute filepath for folder containing expression image files
expression_directory_path = "C:\\Users\\jonat\\Documents\\RoboFace\\expressions8x16\\"
file_extension = bytes(".png", "utf-8")

#get image dimensions from blank (all black pixels) expression
blank_image = Image.open(expression_directory_path + "BLANK.png")
img_width = blank_image.width
img_height = blank_image.height
img_pixels_total = img_width * img_height
blank_image.close()

#make an ordered list of names for each expression so we can create an enum later
expression_names = []
#dict of each expression's pixel data, {expression_name : [data]}
#pixels are read in order of bottom left to top right, in triples of (r, g, b)
expression_image_data = {}


#iterate over all files in expressions folder
directory = (os.fsencode(expression_directory_path))

for file in os.listdir(directory):
    filename = os.fsencode(file)
    
    if file.endswith(file_extension):
        img_data = []
        #add expression name to running list
        expression_name = filename.removesuffix(file_extension)
        expression_names.append(expression_name.decode())
        
        #extract RGB data
        
        image = Image.open(expression_directory_path + filename.decode())
        for y in range(img_height):
            for x in range(img_width):
                pixel = image.getpixel((x,y)) 
                #ignores 4th val, so no transparency used if png
                rgb = (pixel[0], pixel[1], pixel[2])
                if (len(rgb) > 3):
                    print(len(rgb))
                    print(rgb)
                img_data.append(rgb)
        
        expression_image_data[expression_name.decode()] = img_data
        image.close()
        
#print(expression_image_data)
#print(expression_names)

animation_json = json.load(open("animation_data.json", "r"))
print(animation_json)

#get animation and expression metadata
num_codes = len(animation_json["code_to_button"].keys())
num_expressions = len(expression_names)
num_animations = len(animation_json["anim_to_framedata"].keys())
max_anim_frames = animation_json["max_anim_frames"]

#write C++ file to store animation data structures and functions
file = open("animations.h", "w")
#constants for array sizes
file.write("#define TOTAL_CODES " + str(num_codes) + "\n")
file.write("#define MAX_ANIM_FRAMES " + str(max_anim_frames) + "\n")

#add structs for arrays of animation and hex code data
file.write("""
typedef struct animationFrame {
\tint expression;
\tint millis;
};
typedef struct codeMap {
\tchar code[9];
\tint animation;
};
""")

#write enum of all remote buttons
file.write("enum Button {\n")
for button in animation_json["button_to_anim"]:
    file.write("\t" + button + ", \n")
file.write("};\n")

#write enum of all processed expressions
file.write("enum Expression {\n")
for expression in expression_names:
    file.write("\t" + expression + ", \n")
file.write("\tEXP_NONE\n")
file.write("};\n")

#write enum of all animations
file.write("enum Animation { \n")
for animation_name in animation_json["anim_to_framedata"].keys():
    file.write("\t" + animation_name + ", \n")
file.write("\tANIM_NONE\n")
file.write("}; \n")

#write func to create hex code -> animation enum map
#flattens the hex code -> button -> animation to just code -> animation, since the board doesn't need to know the button names
file.write("codeMap codeToAnim[" + str(num_codes) + "] = { \n")
for code in animation_json["code_to_button"].keys():
    button_name = animation_json["code_to_button"][code]
    animation_name = animation_json["button_to_anim"][button_name]
    file.write("\t{\"" + code + "\", " + animation_name + "}, \n")
file.write("}; \n")

#write expression and timing data for each animation
file.write("animationFrame animationData[" + str(num_animations) + "][" +  str(max_anim_frames) + "]= {\n" )
for animation in animation_json["anim_to_framedata"].keys():
    frame_data = animation_json["anim_to_framedata"][animation]
    print(frame_data)
    line = "\t{"
    for frame in frame_data:
        print(frame)
        line += "{" + frame[0] + ", " + str(frame[1]) + "}, "
    line += "},\n"
    file.write(line)
    
file.write("};\n")

#write function for matching incoming code to its associated animation
file.write("""
//find animation associated to IR hex code
int getAnimationFromCode(codeMap codeList[], char code[9]) {
  for (int i = 0; i < TOTAL_CODES; i++ ) {
    if(strcmp(codeList[i].code, code) == 0) {
      return codeList[i].animation;
    }
  }
  //if no match found, return a "no match" number
  return ANIM_NONE;
}
""")


file.close()

file = open("expressions.h", "w")
file.write("""
#define TOTAL_PIXELS """ + str(img_pixels_total) + "\n" + """
typedef struct rgbData {
\tint r;
\tint g;
\tint b;
};
""")
#write pixel data for each expression
file.write("rgbData expressionData["+ str(num_expressions) + "][" + str(img_pixels_total) + "]= {\n")
for expression in expression_names:
    img_data = expression_image_data[expression]
    #{[EXPRESSION], { {0,255,255}, ..., {0, 0, 0} }},
    line = "\t{"

    for pixel in img_data:
        line += "{" + str(pixel[0]) + ", " + str(pixel[1]) + ", " + str(pixel[2]) + "}, "
    
    line += "},\n"
    file.write(line)
    
file.write("};\n")



file.close()
