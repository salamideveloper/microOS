#!/bin/bash

if [ -z "$(git status --porcelain)" ]; then 
    echo "bro there is NOTHING to change are you out of your mind"
    exit 1
fi

echo "enter the commit message just dont say some dumb shit: "
read message

if [ -z "$message" ]; then
    message="Update."
fi

git add .
git commit -m $message

git push origin main --force

echo "All right yeah its done i swear"
echo "Done vro 🥔"