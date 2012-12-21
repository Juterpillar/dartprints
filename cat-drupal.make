; To update a specific project, run the following command from the site root:
;   drush make -y --no-core --contrib-destination=. cat-drupal.make --projects=(name)
;
; To update ALL projects in the build file, exclude the --projects argument,
; but you generally shouldn't be doing that.
;
; Add the --no-cache argument if for any reason you need to prevent Drush
; from re-using its local copies of files it has previously downloaded.

core = 7.x
api = 2
