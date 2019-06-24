#!/bin/bash

################################################################################################################################################################

# @project        Library ▸ Mathematics
# @file           tools/development/start.sh
# @author         Lucas Brémond <lucas@loftorbital.com>
# @license        Apache License 2.0

################################################################################################################################################################

script_directory="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Setup environment

source "${script_directory}/../.env"

# Build Docker image if it does not exist already

if [[ "$(docker images -q ${image_name}:${image_version} 2> /dev/null)" == "" ]]; then

    pushd "${script_directory}/docker" > /dev/null

    ./build.sh

    popd

fi

options=""
command="/bin/bash"

# Setup linked mode

if [[ ! -z $1 ]] && [[ $1 == "--link" ]]; then

    options=""
    command=""

    # Library ▸ Core

    library_core_directory="${project_directory}/../library-core"

    if [[ ! -d ${library_core_directory} ]]
    then

        echo "Library ▸ Core directory [${library_core_directory}s] cannot be found."

        exit 1

    fi

    options="${options} \
    --volume=${library_core_directory}:/mnt/library-core:ro"

    command=" \
    rm -rf /usr/local/include/Library/Core; \
    rm -f /usr/local/lib/liblibrary-core.so*; \
    cp -as /mnt/library-core/include/Library/Core /usr/local/include/Library/Core; \
    cp -as /mnt/library-core/src/Library/Core/* /usr/local/include/Library/Core/; \
    ln -s /mnt/library-core/lib/liblibrary-core.so /usr/local/lib/; \
    ln -s /mnt/library-core/lib/liblibrary-core.so.0 /usr/local/lib/;"

    # Output

    command="${command} \
    /bin/bash"

fi

# Run Docker container

docker run \
--name="${container_name}" \
-it \
--rm \
--privileged \
${options} \
--volume="${project_directory}:/app:rw" \
--volume="${script_directory}/helpers/build.sh:/app/build/build.sh:ro" \
--volume="${script_directory}/helpers/test.sh:/app/build/test.sh:ro" \
--volume="${script_directory}/helpers/debug.sh:/app/build/debug.sh:ro" \
--volume="${script_directory}/helpers/clean.sh:/app/build/clean.sh:ro" \
--workdir="/app/build" \
"${image_name}:${image_version}" \
/bin/bash -c "${command}"

################################################################################################################################################################
