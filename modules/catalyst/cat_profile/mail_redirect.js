/**
 * Event handling for the mail_redirect module
 */


/**
 * Only want to display the email field when "Redirect to:" is selected
 */
function enable_disable_email_field(select_field) {
    if ($(select_field).children("[@selected]").text() == "Redirect to:") {
        $(select_field).parent().parent().parent().find('.mail-redirect-email').removeAttr('disabled');
    }
    else {
        $(select_field).parent().parent().parent().find('.mail-redirect-email').attr('disabled', 'disabled');

    }
}




if (Drupal.jsEnabled) {
    $(document).ready(function() {
        $(".mail-redirect-rule")
            .change(function() {
                enable_disable_email_field(this);
            })
            .each(function() {
                enable_disable_email_field(this);
            });

        $(".mail-redirect-add-mailkey").html('<br /><a href="#add_mailkey" onclick="return false;" class="mail-redirect-add-mailkey">Add mailkey</a>');
        $(".mail-redirect-no-js-new-mailkey").parent().parent().parent().hide();

        var new_mailkey_next_row_even = Drupal.settings.mail_redirect['new_mailkey_next_row_even'];
        var new_mailkey_next_id = 0;

        $(".mail-redirect-add-mailkey").click(function() {
          $(this).parent().find('tbody').append(
            '<tr class="' + (new_mailkey_next_row_even ? 'even' : 'odd') + '" id="new_mailkey' + new_mailkey_next_id + '">' +
              '<td>' + Drupal.settings.mail_redirect['title_field'] + '</td>' +
              '<td>' + Drupal.settings.mail_redirect['rule_field'] + '</td>' +
              '<td>' + Drupal.settings.mail_redirect['email_field'] + '</td>' +
              '<td>' + Drupal.settings.mail_redirect['remove_field'] + '</td>' +
            '</tr>'
          );

          var mid = '#new_mailkey' + new_mailkey_next_id;
          $(mid).highlightFade({speed:1000});

          enable_disable_email_field($(mid).find('.mail-redirect-rule'));

          $(mid).find('.mail-redirect-rule').change(function() {
            enable_disable_email_field(this);
          });


/*
    This is a dirty hack for modifying the class and ID values so Drupal will recognise all new mailkeys

    e.g. turns
               edit-mailkey-rules-mailkey-rule-table-add-mailkey-title
         into
               edit-mailkey-rules-mailkey-rule-table-add-mailkey-X-title

    We could make this code shorter by putting it in a loop, but that would probably just make it more complex!
*/
          var id = $(mid).find('.mail-redirect-title').attr("id");
          var name = $(mid).find('.mail-redirect-title').attr("name");
          id = id.replace(/-title$/, '-' + new_mailkey_next_id + '-title');
          name = name.replace(/\[title\]$/, '[' + new_mailkey_next_id + '][title]');
          $(mid).find('.mail-redirect-title').attr("id", id);
          $(mid).find('.mail-redirect-title').attr("name", name);

          id = $(mid).find('.mail-redirect-rule').attr("id");
          name = $(mid).find('.mail-redirect-rule').attr("name");
          id = id.replace(/-rule$/, '-' + new_mailkey_next_id + '-rule');
          name = name.replace(/\[rule\]$/, '[' + new_mailkey_next_id + '][rule]');
          $(mid).find('.mail-redirect-rule').attr("id", id);
          $(mid).find('.mail-redirect-rule').attr("name", name);

          id = $(mid).find('.mail-redirect-email').attr("id");
          name = $(mid).find('.mail-redirect-email').attr("name");
          id = id.replace(/-email$/, '-' + new_mailkey_next_id + '-email');
          name = name.replace(/\[email\]$/, '[' + new_mailkey_next_id + '][email]');
          $(mid).find('.mail-redirect-email').attr("id", id);
          $(mid).find('.mail-redirect-email').attr("name", name);

          id = $(mid).find('.mail-redirect-remove').attr("id");
          name = $(mid).find('.mail-redirect-remove').attr("name");
          id = id.replace(/-remove$/, '-' + new_mailkey_next_id + '-remove');
          name = name.replace(/\[remove\]$/, '[' + new_mailkey_next_id + '][remove]');
          $(mid).find('.mail-redirect-remove').attr("id", id);
          $(mid).find('.mail-redirect-remove').attr("name", name);

          new_mailkey_next_id = new_mailkey_next_id + 1;
          new_mailkey_next_row_even = !new_mailkey_next_row_even;
        });
    });
};


