/**
 * Let's add the hide button behavior
 */
Drupal.behaviors.aef_easy_view = function() {

  $('.aef_easy_view_wrapper').each(function() {

    $(this).find('.aef-easy-view-preview').each(function() {
      var preview = $(this);

      if(preview.hasClass('aef-easy-view-preview-processed') == false)
      {
        $('a', preview).click(function() {
          preview.hide('normal');
          return false;
         });
        preview.addClass('aef-easy-view-preview-processed');
      }
    });

    //Update the list of formatters according to the pagination selection
    var pagination_select = $(this).find('.aef-easy-view-pagination');
    var formatter_select = $(this).find('.aef-easy-view-nodeformatter');

    aef_easy_view_update_formatter_list(pagination_select, formatter_select);
    pagination_select.change(function() {
      aef_easy_view_update_formatter_list(pagination_select, formatter_select);
    });

  });

}

function aef_easy_view_update_formatter_list(pagination_select, formatter_select)
{

    if(pagination_select.size() > 0)
    {
      var selected_pagination = pagination_select.val();
      var selected_formatter = formatter_select.find('option[value="' + formatter_select.val() + '"]');
      var conf = pagination_select.attr('pagination-' + selected_pagination + '-formatters');
      var defaultconf = pagination_select.attr('pagination-' + selected_pagination + '-formatters-default');

      if(defaultconf != undefined && defaultconf == 'hide')
        formatter_select.find('option').hide();
      else
        formatter_select.find('option').show();

      if(conf != undefined)
      {
        var conf_elts = conf.split(reg=new RegExp("[ ]+", "g"));
        for (var j = 0; j < conf_elts.length; j++) {
          if(conf_elts[j].charAt(0) == '!')
          {
            //Remove this formatter from the list
            conf_elts[j] = conf_elts[j].substring(1, conf_elts[j].length);
            formatter_select.find('option[value="'+ conf_elts[j] +'"]').hide();
          }
          else
          {
            //Add this formatter to the list
            formatter_select.find('option[value="'+ conf_elts[j] +'"]').show();
          }
        }
      }

      //If the selected formatter value is hidden, set it to the default value
      if(selected_formatter.css('display') == 'none')
      {
        var options = formatter_select.find('option');
        for(var i = 0; i < options.size(); i++)
        {
          if(options.eq(i).css('display') != 'none')
          {
            formatter_select.val(options.eq(i).val());
          }
        }
      }
    }
}
