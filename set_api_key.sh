#!/bin/bash

# --- USAGE --- 
# To set the API key from the text file as an environment variable, run the following commands:
# chmod +x set_api_key.sh
# source ./set_api_key.sh


export CC_API_KEY=$(cat API_KEY.txt)
echo $CC_API_KEY