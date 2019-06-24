#!/bin/bash

################################################################################################################################################################

# @project        Library ▸ Mathematics
# @file           tools/ci/test.sh
# @author         Lucas Brémond <lucas@loftorbital.com>
# @license        Apache License 2.0

################################################################################################################################################################

script_directory="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

project_directory="${script_directory}/../.."
development_directory="${project_directory}/tools/development"

source "${project_directory}/tools/.env"

docker run \
--rm \
--volume="${project_directory}:/app:rw" \
--volume="/app/build" \
--volume="${development_directory}/helpers/build.sh:/app/build/build.sh:ro" \
--volume="${development_directory}/helpers/test.sh:/app/build/test.sh:ro" \
--workdir="/app/build" \
${image_name}:${image_version} \
/bin/bash -c "/app/build/build.sh && /app/build/test.sh"

################################################################################################################################################################
