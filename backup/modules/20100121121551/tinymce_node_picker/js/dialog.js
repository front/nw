// $id$
Drupal.behaviors.tinyMCENodePicker = function (context) {	
	var parentWindow = (window.opener) ? window.opener : window.parent;
	var instanceId = parentWindow.Drupal.wysiwyg.activeId;
	var editor = parentWindow.Drupal.wysiwyg.instances[instanceId].editor;
	var content = '';
	
	// Fetch text selection, different for different editors.
	switch(editor) {
		case "tinymce":
			content = parentWindow.tinyMCE.activeEditor.selection.getContent();
			break;
	}
	
	// Capture click on "Insert link".
	$(".views-field a").click(function(){
		// Insert content.
		if (parent) {
			var title = $(this).parent().parent().children("td:eq(0)").text().replace(/^\s+|\s+$/g, "");
    	var link = "<a href=\""+ $(this).attr("href") +"\" title=\""+ title +"\">"+ content +"</a>";
    	parentWindow.Drupal.wysiwyg.instances[instanceId].insert(link);
    	parentWindow.Drupal.wysiwyg.instances[instanceId].closeDialog(window);
  	}
  	else {
    	alert("The link cannot be inserted because the parent window cannot be found.");
  	}
    return false;
	});
	
	// Capture click on "Insert link".
	$("ul.menu a").click(function(){
		// Insert content.
		if (parent) {
    	var link = "<a href=\""+ $(this).attr("href") +"\" title=\""+ $(this).attr("title") +"\">"+ content +"</a>";
    	parentWindow.Drupal.wysiwyg.instances[instanceId].insert(link);
    	parentWindow.Drupal.wysiwyg.instances[instanceId].closeDialog(window);
  	}
  	else {
    	alert("The link cannot be inserted because the parent window cannot be found.");
  	}
    return false;
	});
};