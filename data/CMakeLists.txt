set(gaia_DATA_FILES
	font.png
)

# targets
if(WITH_INSTALL)
	install(FILES ${gaia_DATA_FILES} DESTINATION ${DATADIR})
endif(WITH_INSTALL)
