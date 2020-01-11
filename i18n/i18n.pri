TRANSLATIONS = \
    $$PWD/XaoS_cs.ts \
    $$PWD/XaoS_de.ts \
    $$PWD/XaoS_es.ts \
    $$PWD/XaoS_fr.ts \
    $$PWD/XaoS_hu.ts \
    $$PWD/XaoS_it.ts \
    $$PWD/XaoS_pt.ts \
    $$PWD/XaoS_ro.ts \
    $$PWD/XaoS_ru.ts

OTHER_FILES = TRANSLATIONS

updateqm.input = TRANSLATIONS
updateqm.output = $$PWD/${QMAKE_FILE_BASE}.qm
updateqm.commands = lrelease ${QMAKE_FILE_NAME} -qm ${QMAKE_FILE_OUT}
updateqm.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += updateqm
