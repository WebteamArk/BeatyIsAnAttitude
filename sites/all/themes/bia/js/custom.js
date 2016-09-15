
(function($) {
	//*****************************
  // Document Ready START
  //*****************************	
	$( document ).ready(function(){
		
		//Arrays for the colors for the background and gradient
		var pagecolors = ["#ff0000", "#ffff00", "#0000ff","#00ff00"];
		
		if (typeof(Drupal.settings.bia_colors) == 'object') {
			pagecolors = Drupal.settings.bia_colors; 
		}
		
		//Percentage how much the color needs to be changed, negative is darker, positive lighter
		$colorChangePercantage = -10;

		//Creating an index for the lenght of the array
		$index = 0;

		//Variables for the fading time in seconds
		$fadetime = 8000;
		var fadeTimeItems = $fadetime /= 2;

		//Fading the pen only at the beginning
		$('#pen').fadeTo(0,0);
	  	$('#pen').fadeTo(fadeTimeItems,1);
	  	
	  	if ($('body.node-type-product .field-name-field-color .field-item').length) {
		  	// The node to be monitored
		  	var target = $( "body.node-type-product .commerce-product-field-field-color").parent()[0];
	
		  	// Create an observer instance
		  	var observer = new MutationObserver(function( mutations ) {
		  	  mutations.forEach(function( mutation ) {
		  	    var newNodes = mutation.addedNodes; // DOM NodeList
		  	    if( newNodes !== null ) { // If there are new nodes added
		  	    	var $nodes = $( newNodes ); // jQuery set
		  	    	$nodes.each(function() {
		  	    		var $node = $( this );
		  	    		if( $node.hasClass( "commerce-product-field-field-color" ) ) {
		  	    			// do something
		  	    			updateColorPalette();
		  	    		}
		  	    	});
		  	    }
		  	  });    
		  	});
	
		  	// Configuration of the observer:
		  	var config = { 
		  		childList: true, 
		  	};
		  	 
		  	// Pass in the target node, as well as the observer options
		  	observer.observe(target, config);
	  	}
	  	
	  	function updateColorPalette() {
	  		if ($('body.node-type-product .field-name-field-color .field-item').length) {
				pagecolors = ["#"+$('body.node-type-product .field-name-field-color .field-item').text()];
				$index = pagecolors.length -1;
				changeColor();
			}
	  	}
	  	
	  	
	  	//match color to the nailiner color if we are on product view page
	  	if ($('body.node-type-product .field-name-field-color .field-item').length) {
	  		updateColorPalette();
		} else {
			//or keep changing colours every $fadetime seconds otherwise
			changeColor();
			window.setInterval(function(){
	  			changeColor();
			}, $fadetime);
		}

		//Function to change the colors
		function changeColor (){			
			$pageColor = pagecolors[$index];
			// Darken a color
			$gradiantcolor = alterColorIntensity($pageColor,$colorChangePercantage);

			//Resetting the index if it hits the end
			if ($index < pagecolors.length -1 ) {
				$index += 1;
			}
			else{
				$index = 0;
			}			
			if ($('#Laag_1').length) {
				//background color of svg
  				$('#background').fadeTo('slow', 0, function()
					{
					    $(this).css('fill', $pageColor);
					}).fadeTo(fadeTimeItems, 1 , 'linear');	  			
	  			//Color for gradient, adding the html code for the gradient inside the svg html code
  				$('.st0').fadeTo('slow', 0, function()
					{
						$( "#svgGradient" ).html( '<stop  offset="0" style="stop-color:#FFFFFF;stop-opacity:0"/> <stop  offset="'+$index+'" style="stop-color:'+$gradiantcolor+'"/>' );

					}).fadeTo(fadeTimeItems, 1 , 'linear');	  			
	  		}
		}
		
		// Function to darken a color
		function alterColorIntensity(color, percent) {

			//Getting value's of every color channel
		    var R = parseInt(color.substring(1,3),16);
		    var G = parseInt(color.substring(3,5),16);
		    var B = parseInt(color.substring(5,7),16);

		    //Adding the percentage to the color
		    R = parseInt(R * (100 + percent) / 100);
		    G = parseInt(G * (100 + percent) / 100);
		    B = parseInt(B * (100 + percent) / 100);

		    //If the value is bigger then the maximum allowed value for a color channel it is set at the maximum
		    R = (R<255)?R:255;  
		    G = (G<255)?G:255;  
		    B = (B<255)?B:255;

		    // Creatinga  variable for converting the rgb to a hex value
		    var hexValue = rgbToHex(R, G, B);
		    return  hexValue;
		}

		//Function to check the hex value's
		function componentToHex(c) {
		    var hex = c.toString(16);
		    return hex.length == 1 ? "0" + hex : hex;
		}

		//Function to convert rgb to hex
		function rgbToHex(r, g, b) {
		    return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
		}
	});
	//*****************************
  // Document Ready END
  //*****************************	


})(jQuery);