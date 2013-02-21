# See LICENSE file for copyright and license details.

include config.mk

SRC = ${NAME}.c
OBJ = ${SRC:.c=.o}
DOC = ${NAME}.1
CONF= config.h

all: options ${NAME}

options:
	@echo ${NAME} build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

${NAME}: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f ${NAME} ${OBJ} ${NAME}-${VERSION}.tar.gz

dist: clean
	@echo creating dist tarball
	@mkdir -p dist
	@git archive -o dist/${NAME}-${VERSION}.tar.gz --prefix="${NAME}-${VERSION}/" -9 v${VERSION}
	@gpg --detach-sign dist/${NAME}-${VERSION}.tar.gz

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f ${NAME} ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/${NAME}
	@echo installing manual page to ${DESTDIR}${PREFIX}/man1
	@mkdir -p  ${DESTDIR}${MANPREFIX}/man1
	@sed "s/VERSION/${VERSION}/g" < ${DOC} > ${DESTDIR}${MANPREFIX}/man1/${DOC}
	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/${DOC}

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/${NAME}
	@echo removing manual page from ${DESTDIR}${PREFIX}/man1
	@rm -f ${DESTDIR}${MANPREFIX}/man1/${DOC}


.PHONY: all options clean dist install uninstall
