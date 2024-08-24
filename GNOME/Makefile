NAME=arkpets-integration
DOMAIN=harryh.cn

.PHONY: all pack install clean

all: dist/extension.js

node_modules: package.json
	pnpm install

dist/extension.js: node_modules
	npx tsc

dist/$(NAME)@$(DOMAIN).zip: dist/extension.js
	@cp metadata.json dist/
	@(cd dist && zip ./$(NAME)@$(DOMAIN).zip -9r .)

pack: dist/$(NAME)@$(DOMAIN).zip

install: dist/$(NAME)@$(DOMAIN).zip
	@gnome-extensions install --force ./dist/arkpets-integration@harryh.cn.zip

clean:
	@rm -rf dist node_modules