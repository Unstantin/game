SRC_DIR = src

GITHUB_DIR = github
GITHUB_SRC_DIR := $(strip $(GITHUB_DIR))/src
GITHUB_IMG_DIR := $(strip $(GITHUB_DIR))/img

################################

.PHONY: all github

all:  
	@cd $(SRC_DIR) && $(MAKE)

github:
	@cd $(SRC_DIR) && $(MAKE) github-src
	@cp -v Makefile $(GITHUB_DIR)
	@rm -rf $(GITHUB_IMG_DIR) 
	@cp -vr img $(GITHUB_DIR)
	
