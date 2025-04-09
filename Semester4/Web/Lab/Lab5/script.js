$(document).ready(function() {
    const $container = $('#gradient-container');
    let isDragging = false;
    let dragDistance = 0;
    let startX = 0;
    let startY = 0;
    let transitionSpeed = 0.3; // Default transition speed in seconds
    
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
            // For longer drags, make transition slower (max 3s)
            // For shorter drags, make transition faster (min 0.05s)
            transitionSpeed = Math.max(0.05, Math.min(3, dragDistance / 100));
            
            console.log("Dragging - Distance: " + dragDistance + ", Speed: " + transitionSpeed + "s");
        }
        
        // Apply the transition speed and center position
        $container.css({
            'transition': `background ${transitionSpeed}s ease`,
            'background': `radial-gradient(circle at ${posX}% ${posY}%, rgb(255, 0, 0), rgb(0, 255, 238), rgb(0, 0, 0))`
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