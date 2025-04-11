$(document).ready(function() {
    const $container = $('#gradient-container');
    let isDragging = false;
    let dragDistance = 0;
    let startX = 0;
    let startY = 0;
    let opacity = 0.05;
    
    // Update gradient center based on mouse position
    $(document).on('mousemove', function(e) {
        const mouseX = e.pageX;
        const mouseY = e.pageY;
        
        // Calculate the position in percentage for the gradient
        const posX = (mouseX / $(window).width()) * 100;
        const posY = (mouseY / $(window).height()) * 100;
        
        // If dragging, calculate the distance and adjust transition speed
        if (isDragging) {
            const dx = mouseX - startX;
            const dy = mouseY - startY;
            const newDistance = Math.sqrt(dx * dx + dy * dy);
            
            // Update drag distance - remove the min cap to make it more noticeable
            dragDistance = newDistance;
            
            // Map drag distance to transition speed
            // For shorter drags, make transition faster (min 0.05s)
            opacity = Math.max(0.05, Math.min(1, dragDistance / 1000));
            
            console.log("Dragging - Distance: " + dragDistance + ", Opacity: " + opacity);
        }
        
        // Apply the transition speed and center position
        $container.css({
            'background': `radial-gradient(circle at ${posX}% ${posY}%, rgb(255, 0, 0), rgb(0, 0, 238, ${opacity}))`
        });
    });
    
    // Start dragging
    $(document).on('mousedown', function(e) {
        isDragging = true;
        startX = e.pageX;
        startY = e.pageY;
        dragDistance = 0;
        console.log("Drag started");
        
        // Add a visual indicator that dragging is active
        $('body').css('cursor', 'grabbing');
    });
    
    // Stop dragging
    $(document).on('mouseup', function() {
        isDragging = false;
        console.log("Drag ended");
        
        // Restore cursor
        $('body').css('cursor', 'default');
    });
    
    // Handle edge cases
    $(document).on('mouseleave', function() {
        if (isDragging) {
            console.log("Mouse left window while dragging");
            isDragging = false;
            $('body').css('cursor', 'default');
        }
    });
}); 