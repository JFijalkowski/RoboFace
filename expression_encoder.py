from PIL import Image
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
print(expression_names)

#write C++ file
file = open("expressions.h", "w")

file.write("#include <map>\n")
file.write("#include <vector>\n")
file.write("#include <tuple>\n")
file.write("#define TOTAL_PIXELS " + str(img_pixels_total) + "\n")

#write enum of all processed expressions
file.write("// Enum of expressions \n")
file.write("enum Expression {\n")
for expression in expression_names:
    file.write("\t" + expression + ", \n")
file.write("};\n")

#write enum of all animations (de-hard-code this later pls)
file.write("""enum Animation {
	ANIM_NEUTRAL,
	ANIM_HAPPY,
	ANIM_SUNGLASSES,
	ANIM_CRYING,
	ANIM_BUFFERING,
	ANIM_OWO,
	ANIM_SLEEPING,
	ANIM_RAINBOW,
	ANIM_SCREENTEST,
	ANIM_BSOD1,
	ANIM_BSOD2,
	ANIM_HYPNO,
	ANIM_BLUSH,
	ANIM_HEART,
	ANIM_SUS,
    ANIM_SAD,
	ANIM_HMM,
	ANIM_VEXED,
	ANIM_IRRITATED,
	ANIM_ANGRY
};""")

#write expression data to map
file.write("/**typedef std::tuple<int, int, int> rgb_values;\n")
file.write("typedef std::vector<rgb_values> img_rgb_data;\n")
file.write("typedef std::map<Expression, img_rgb_data> rgb_data_map;\n")
file.write("rgb_data_map expression_data_map = {\n")
#write pixel data list for each expression
for expression in expression_names:
    img_data = expression_image_data[expression]
    #{[EXPRESSION], { {0,255,255}, ..., {0, 0, 0} }},
    line = "\t{" + expression + ", {"

    for pixel in img_data:
        line += "{" + str(pixel[0]) + ", " + str(pixel[1]) + ", " + str(pixel[2]) + "}, "
    
    line += "}},\n"
    file.write(line)
    
file.write("};**/\n")

file.close()
