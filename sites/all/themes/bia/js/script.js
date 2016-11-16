/**
 * @file
 * A JavaScript file for the theme.
 *
 * In order for this JavaScript to be loaded on pages, see the instructions in
 * the README.txt next to this file.
 */

// JavaScript should be made compatible with libraries other than jQuery by
// wrapping it with an "anonymous closure". See:
// - http://drupal.org/node/1446420
// - http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth
(function ($, Drupal, window, document, undefined) {

Drupal.behaviors.selectWidget = {
  attach: function (context, settings) {
	  $('.select-widget:not(.processed)').each(function(){
			if ($(this).find('select').val() == '_none' || $(this).find('select').val() == 'All') $(this).find('.select-text').addClass('none'); 
			else $(this).find('.select-text').removeClass('none');
			$(this).find('.select-text').val($(this).find('option:selected').text());
			
			
			//mutations done by other scripts (not only jQuery)
			var target = $(this).find('select')[0];
			var observer = new MutationObserver(function( mutations ) {
				$(mutations[0].target).parent().find('.select-text').val($(mutations[0].target).find('option:selected').text());
			});
			var config = { 
				childList: true,
				attributes: true,
				characterData: true,
			};
			observer.observe(target, config);
			
			$(this).change(function(){
				if ($(this).val() == '_none' || $(this).val() == 'All') $(this).parent().find('.select-text').addClass('none'); 
				else $(this).parent().find('.select-text').removeClass('none');
				$(this).parent().find('.select-text').val($(this).find('option:selected').text());
			});
			$(this).keyup(function(){
				if ($(this).val() == '_none' || $(this).val() == 'All') $(this).parent().find('.select-text').addClass('none'); 
				else $(this).parent().find('.select-text').removeClass('none');
				$(this).parent().find('.select-text').val($(this).find('option:selected').text());
			});
			$(this).addClass('processed');
		});
	}
};


Drupal.behaviors.updateQuantity = {
  attach: function (context, settings) {
    $('#views-form-commerce-cart-form-default .form-type-textfield input', context).change(function(){
    	$('#edit-submit',context).mousedown();
    });
    
    $('.group-right .commerce-product-field .field-label').click(function(){
    	$(this).parent().parent().toggleClass('active');
    });
 }
};

// Place your code here.
$(document).ready(function(){
	$('.menutrigger').click(function(){
		$(this).toggleClass('active');
	});
	
	$('.view-finishblock .views-row, #block-views-faq-block-1 .views-row').click(function(){
		$(this).toggleClass('active');
	});
	
	$(document).on('click', 'a.skipdown, a.skipdown2', function(event){
	    event.preventDefault();
	    $('html, body').animate({
	        scrollTop: $( $.attr(this, 'href') ).offset().top
	    }, 400);
	});

});


})(jQuery, Drupal, this, this.document);
