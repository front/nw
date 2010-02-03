// $Id: nodepicker.js,v 1.4 2009/12/03 07:18:33 blixxxa Exp $

Drupal.wysiwyg.plugins["nodepicker"] = {
  /**
   * Execute the button.
   */
  invoke: function(data, settings, instanceId) {
  	// Make sure a selection exists.
  	if(typeof data.content == "undefined" || data.content.length == 0) {
  		return false;
  	}
  	
  	// Options to pass to the dialog.
		var options = { id: instanceId, content: data.content };
 
    // Open dialogue.
    Drupal.wysiwyg.instances[instanceId].openDialog(settings.dialog, options);
  }
};