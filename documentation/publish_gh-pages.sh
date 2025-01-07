#!/usr/bin/env bash

#set -euo pipefail

exiterr (){ printf "$@\n"; exit 1;}


trap cleanup EXIT

source publish.env

function cleanup {
  #rm -rf "${CLONE_DIRECTORY}"
  echo "Deleted temp working directory ${CLONE_DIRECTORY}"
}



check_params() {
    echo "Please verify the following parameters:"
    echo "---------------------------------------"
    echo "PROHIBITED_REMOTES: $PROHIBITED_REMOTES"
    echo "DOCUMENTATION_SOURCE_BRANCH: $DOCUMENTATION_SOURCE_BRANCH (the branch which the documentaiton will originate from)"
    echo "PUBLISH_BRANCH: $PUBLISH_BRANCH (The branch that the generated docs directory will publish to)"
    echo "PUBLISH_REMOTE: $PUBLISH_REMOTE (The repository uri where the documentaiton/gh-pages will be published)"
    echo "PUBLISH_COMMIT_MESSAGE: $PUBLISH_COMMIT_MESSAGE"
    echo "---------------------------------------"

    read -p "Are these values correct? (yes/no): " response
    if [[ "$response" != "yes" ]]; then
        echo "Exiting: Modify the 'publish.env' file and try again."
        exit 1
    fi
}

check_params


SCRIPT_DIRECTORY="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
DOCS_DIRECTORY="${SCRIPT_DIRECTORY}/docs"

printf "Creating temporary workspace directory...\n"
CLONE_DIRECTORY="$(mktemp -d)"


#git_url="$(git remote get-url ${PUBLISH_REMOTE})"
git_url="${PUBLISH_REMOTE}"


#if [[ ! -d "${DOCS_DIRECTORY}" ]]; then
#    exiterr "ERROR: docs directory: ${DOCS_DIRECTORY} does not exist.  Did you call 'make build_gh-pages'" 
#fi


if [[ $PROHIBITED_REMOTES == *"$git_url"* ]]; then
    exiterr "ERROR: Prohibited to publish to origin: ${git_url}, create a fork or change the origin and try again."
fi


read -p "Should the remote publish branch: ${PUBLISH_BRANCH} be deleted? (y/n): " answer
if ! [[ $answer == [Yy] ]]; then
    git push ${PUBLISH_REMOTE} --delete "${PUBLISH_BRANCH}"
    git branch -d "${PUBLISH_BRANCH}"
fi


cd "${CLONE_DIRECTORY}"
printf "Cloning adore to: ${CLONE_DIRECTORY}/adore\n"
git clone --depth 1 --single-branch -b "${DOCUMENTATION_SOURCE_BRANCH}" "${git_url}"
``

cd ${CLONE_DIRECTORY}/adore
git checkout --orphan "${PUBLISH_BRANCH}"

{
cd ${CLONE_DIRECTORY}/adore/documentation
make build
}
cd ${CLONE_DIRECTORY}/adore
git reset
pwd
git status
mv ${CLONE_DIRECTORY}/adore/documentation/docs .
git add docs --force

git commit -am "${PUBLISH_COMMIT_MESSAGE}"

git remote add publish "${PUBLISH_REMOTE}"
git push --force --set-upstream publish "${PUBLISH_BRANCH}" | true
git push publish "${PUBLISH_BRANCH}" 
