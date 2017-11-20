cd ..
del/q glsleditor.zip
mkdir glsleditortmp
copy GLSLEditor\GLSLEditor\license.txt glsleditortmp
copy GLSLEditor\GLSLEditor\Release\SciTE.exe glsleditortmp
copy GLSLEditor\GLSLEditor\Release\glsl.exe glsleditortmp
copy GLSLEditor\GLSLEditor\Release\SciLexer.dll glsleditortmp
copy GLSLEditor\GLSLEditor\src\*.properties glsleditortmp
copy GLSLEditor\GLSLEditor\lua\COPYRIGHT glsleditortmp\luaCOPYRIGHT
del/q wscite\Embedded.properties
copy GLSLEditor\GLSLEditor\doc\*.html glsleditortmp
copy GLSLEditor\GLSLEditor\doc\*.png glsleditortmp
copy GLSLEditor\GLSLEditor\doc\*.jpg glsleditortmp
copy GLSLEditor\GLSLEditor\doc\*.1 glsleditortmp
7z a -tzip glsleditor.zip glsleditortmp\*.*
del/q glsleditortmp\*.*
rmdir glsleditortmp
cd GLSLEditor
