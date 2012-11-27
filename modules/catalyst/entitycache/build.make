; See PATCHES.txt
;
; To update/recreate this module, first run the following command:
;   drush make --no-core build.make
; (answer 'y' to the question, or pass -y in the command)
;
; Then move the resulting module from sites/all/modules/contrib/*
;
; e.g.:
; drush make --no-core build.make -y
; rsync -r --remove-source-files sites/all/modules/contrib/* ..
; find sites/ -type d | sort -r | xargs rmdir

core = 7.x
api = 2


; Entity cache
projects[entitycache][type] = module
projects[entitycache][subdir] = contrib
projects[entitycache][version] = 1.x-dev
; PDOException:Invalid text representation when attempting to load an entity with a string ID
; http://drupal.org/node/1851398
projects[entitycache][patch][] = "http://drupal.org/files/entitycache-numeric-ids.patch"
