// $Id: dialog.js,v 1.13 2010/01/20 07:48:30 blixxxa Exp $

Drupal.behaviors.tinyMCENodePicker = function (context) {
  var parentWindow = (window.opener) ? window.opener : window.parent;
  if (parentWindow && parentWindow.Drupal) {
    var instanceId = parentWindow.Drupal.wysiwyg.activeId;
    var editor = parentWindow.Drupal.wysiwyg.instances[instanceId].editor;
    var content = '';

    // Fetch text selection, different for different editors.
    switch(editor) {
      case "tinymce":
        content = parentWindow.tinyMCE.activeEditor.selection.getContent();
      break;
    }
    
    // Strip anchors from content.
    content = content.replace(/<[//]{0,1}(A|a)[^><]*>/g, "");

    // Capture click on "Insert link".
    $(".views-field a").click(function() {
      // Insert content.
      if (parentWindow && parentWindow.Drupal) {
        var title = $(this).parent().parent().children("td:eq(0)").text().replace(/^\s+|\s+$/g, "");
        var link = "<a href=\""+ $(this).attr("href") +"\" title=\""+ title +"\">"+ ((content == '') ? title : content) +"</a>";
        
        // If no selection exists and cursor is located inside anchor tag, update link.
        if(content != '') {
          switch(editor) {
            case "tinymce":
              parentWindow.tinyMCE.activeEditor.execCommand('mceInsertLink', false, $(this).attr("href"));
            break;
              
            default:
              parentWindow.Drupal.wysiwyg.instances[instanceId].insert(link)
            break;
          }
        }
        else {
          parentWindow.Drupal.wysiwyg.instances[instanceId].insert(link)
        }
        parentWindow.Drupal.wysiwyg.instances[instanceId].closeDialog(window);
      }
      else {
        alert(Drupal.t("The link cannot be inserted because the parent window cannot be found."));
      }
      return false;
    });

    // Capture click on "Insert link".
    $("ul.menu a").click(function() {
        // Insert content.
      if (parentWindow && parentWindow.Drupal) {
				var title = $(this).attr("title");
				var link = "<a href=\""+ $(this).attr("href") +"\" title=\""+ title +"\">"+ ((content == '') ? title : content) +"</a>";
				
				// If no selection exists and cursor is located inside anchor tag, update link.
				if(content != '') {
  				switch(editor) {
            case "tinymce1":
              parentWindow.tinyMCE.activeEditor.execCommand('mceInsertLink', false, $(this).attr("href"));
              break;
              
            default:
              parentWindow.Drupal.wysiwyg.instances[instanceId].insert(link)
              break;
          }
        }
        else {
          parentWindow.Drupal.wysiwyg.instances[instanceId].insert(link)
        }
        parentWindow.Drupal.wysiwyg.instances[instanceId].closeDialog(window);
      }
      else {
        alert(Drupal.t("The link cannot be inserted because the parent window cannot be found."));
      }
      return false;
    });
  }
};