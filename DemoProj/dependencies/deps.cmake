if(NOT DEPS_INCLUED)

set(CV_LIBS
	opencv_core340
	opencv_imgproc340
	opencv_highgui340
	opencv_imgcodecs340
	opencv_videoio340
    )
foreach(lib ${CV_LIBS})
	set(CV_LINK_LIBS
		${CV_LINK_LIBS}
		debug ${lib}d
		optimized ${lib}
		)
endforeach()

set(OV_LIBS
	inference_engine
	inference_engine_legacy
	inference_engine_ir_reader
	)
foreach(lib ${OV_LIBS})
	set(OV_LINK_LIBS
		${OV_LINK_LIBS}
		debug ${lib}d
		optimized ${lib}
		)
endforeach()

set(DEPS_INCLUED ON)
endif()    #DEPS_INCLUED