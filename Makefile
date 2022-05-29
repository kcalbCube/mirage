.SILENT: 
default:
	cmake --build build --target all_pch -j 3
	cmake --build build --target all -j 3
unity:
	cmake --build build --target all_pch -j 3
	cmake --build build --target all_unity -j 3
clean:
	cmake --build build --target clean
	cmake --build build --target clean_cotire
