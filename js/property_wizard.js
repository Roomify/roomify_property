(function ($) {

/*
 * Default settings for all BAT datepickers that come in pairs.
 */
Drupal.behaviors.propertyWizard = {
  attach: function(context) {

    // Disable submit buttons.
    $('input[name=next]').prop('disabled', true);
    $('input[value="create property and add details"]').prop('disabled', true);

    // Disable submit buttons if all fields are not filled in.
    $('#modalContent form :input').on('change input', function() {

      // Property name step.
      if ($('input[name=property_name]').length) {
        if ($.trim($('input[name=property_name]').val()).length && $('input[name=property_type]:checked').val()) {
          $('input[name=next]').prop('disabled', false);
        }
        else {
          $('input[name=next]').prop('disabled', true);
        }
      }
      // Type info step.
      else if ($('input[name=max_occupants]').length) {
        if ($.trim($('input[name=max_occupants]').val()).length && $.trim($('input[name=default_price]').val()).length) {
          $('input[value="create property and add details"]').prop('disabled', false);
        }
        else {
          $('input[value="create property and add details"]').prop('disabled', true);
        }
      }
    });
  }
};
})(jQuery);
