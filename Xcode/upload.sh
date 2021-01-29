#!/usr/bin/env bash
# Create archive if it doesn't already exist
# Generally not needed, and just a refresh is performed
if [ ! -f ./Archive.zip ];
then
  echo "Creating Archive.zip"
else
  echo "Updating existing archive"
fi

zip -u -r Archive.zip index.js config.js constants.js src device node_modules package.json project.json
echo "Uploading Archive.zip to US, UK, and AU AWS Lambdas";

# Upload to all the language lambdas (PatriotSmartHomeUS, *UK, *AU)
#aws lambda update-function-code --function-name PatriotSmartHomeUS --zip-file fileb://Archive.zip
#aws lambda update-function-code --function-name PatriotSmartHomeUK --zip-file fileb://Archive.zip --profile uk
#aws lambda update-function-code --function-name PatriotSmartHomeAU --zip-file fileb://Archive.zip --profile au

# Dev/test lambda
aws lambda update-function-code --function-name rvSmartHome --zip-file fileb://Archive.zip

echo Remember to version the Lambdas after testing and before submitting.
