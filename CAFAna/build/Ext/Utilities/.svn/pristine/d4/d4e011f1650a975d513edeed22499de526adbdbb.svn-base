# Get directory of this script
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# pushd $DIR
cd $DIR

# XMLtoDict download
wget --quiet https://raw.githubusercontent.com/martinblech/xmltodict/579a00520315e30681e0f0f81de645ce5680ed47/xmltodict.py
chmod +x xmltodict.py

# Requests
if [ -d requests ]; then exit; fi

wget --quiet --output-document requests.zip https://github.com/kennethreitz/requests/archive/6d0017ee6a242433b2a05e78ae57ae0b84ad4b13.zip
unzip -qq requests.zip
mv requests-6d0017ee6a242433b2a05e78ae57ae0b84ad4b13/requests ./
rm -rf requests-6d0017ee6a242433b2a05e78ae57ae0b84ad4b13
rm requests.zip
