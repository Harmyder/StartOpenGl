To generate these files for another version one needs to use [glLoadGen](https://bitbucket.org/alfonse/glloadgen/wiki/Home)

Use Lua 5.1 because that will the job for sure, while with 5.3 the script crashes.

`\glLoadGen_2_0_5>..\lua5.1.exe LoadGen.lua core_4_4 -style=pointer_cpp -spec=gl -version=4.4 -profile=core -stdext=gl_ubiquitous.txt -stdext=gl_plat_3_3.txt`

