cd ..
del/q glsleditor.zip
mkdir glsleditortmp
copy GLSLEditor\license.txt glsleditortmp
copy GLSLEditor\Release\SciTE.exe glsleditortmp
copy GLSLEditor\Release\glsl.exe glsleditortmp
copy GLSLEditor\Release\SciLexer.dll glsleditortmp
copy GLSLEditor\src\*.properties glsleditortmp
copy GLSLEditor\lua\COPYRIGHT glsleditortmp\luaCOPYRIGHT
del/q wscite\Embedded.properties
copy GLSLEditor\doc\*.html glsleditortmp
copy GLSLEditor\doc\*.png glsleditortmp
copy GLSLEditor\doc\*.jpg glsleditortmp
copy GLSLEditor\doc\*.1 glsleditortmp
7z a -tzip glsleditor.zip glsleditortmp\*.*
del/q glsleditortmp\*.*
rmdir glsleditortmp
cd GLSLEditor
