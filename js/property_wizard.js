(function ($) {

/**
 * Property Wizard JS.
 */
Drupal.behaviors.propertyWizard = {
  attach: function(context) {

    /**
     * Auto modal sizing stuff.
     */
    var winHeight = $(window).height();
    var winWidth = $(window).width();

    // Use the additional pixels for creating the width and height.
    $('div.ctools-modal-content', context).css({
      'min-width': Drupal.CTools.Modal.currentSettings.modalSize.width,
      'min-height': Drupal.CTools.Modal.currentSettings.modalSize.height,
      'width': 'auto',
      'height': 'auto',
      'max-height': (winHeight / 2) * 1.6 + 'px',
      'max-width': (winWidth / 2) * 1.6 + 'px',
      'overflow': 'none'
    });

    $('div#modal-content', context).css({
      'height': 'auto',
      'width': 'auto',
    });

    $('#modalContent').css({'width': 'auto'});
    $('div.ctools-modal-content .modal-content', context).css("overflow", "hidden");

    // Trigger a resize to re-center the modal.
    $(window).trigger('resize');


    /**
     * Messing aboot wi' the bootons.
     */

    // Disable submit buttons.
    $('input[name=next]').prop('disabled', true);
    if ($.trim($('input[name="types_container[0][name]"]').val()).length) {
      $('input[value="create property and add details"]').prop('disabled', false);
    }
    else {
      $('input[value="create property and add details"]').prop('disabled', true);
    }

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

      // Locanda type info step.
      else if ($('input[name="types_container[0][name]"]').length) {
        $('input[value="create property and add details"]').prop('disabled', false);

        // For each type container, if any input has text, all must have text.
        for ($i = 0; $i < $('div.type-container').length; $i++) {
          // First find out if any input is filled.
          var filled = false;
          $('div#type-container-' + $i).find(':input').each(function(index) {
            if ($.trim($(this).val()).length) {
              filled = true;
            }
          });

          if (filled) {
            $('div#type-container-' + $i).find(':input').each(function(index) {
              if (!$.trim($(this).val()).length) {
                $('input[value="create property and add details"]').prop('disabled', true);
              }
            });
          }
        }
      }
    });
  }
};
})(jQuery);
