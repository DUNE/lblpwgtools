all:
	@echo -n "Pulling external libraries... "
	@libs/PullExternalLibs.sh
	@echo "DONE."
	
clean:
	@echo -n "Removing external libraries... "
	@if [ -e libs/xmltodict.py ]; then rm libs/xmltodict.py; fi
	@if [ -d libs/requests ];     then rm -rf libs/requests; fi
	@echo "DONE."
	
	@echo -n "Removing .pyc files... "
	@find . -maxdepth 2 -name '*.pyc' -exec rm -r {} \;
	@find . -maxdepth 2 -name '*.py.*' -exec rm -r {} \;
	@echo "DONE."
