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


; Rubik admin theme
projects[rubik][type] = theme
projects[rubik][subdir] = catalyst
projects[rubik][version] = 4.x-dev
; Provide theme setting to render form field descriptions inline: http://drupal.org/node/1730844
projects[rubik][patch][] = "http://drupal.org/files/rubik_inline_field_desc-1730844-10.patch"
; Support Error classes on table rows http://drupal.org/node/1879560
projects[rubik][patch][] = "http://drupal.org/files/rubik-table-error-striping.patch"
