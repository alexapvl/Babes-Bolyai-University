document.addEventListener('DOMContentLoaded', function() {
    // Menu data structure
    const menuData = [
        {
            title: "Products",
            items: [
                { name: "Electronics", link: "#electronics" },
                { name: "Clothing", link: "#clothing" },
                { name: "Home & Garden", link: "#home-garden" },
                { name: "Books", link: "#books" }
            ]
        },
        {
            title: "Services",
            items: [
                { name: "Consulting", link: "#consulting" },
                { name: "Web Development", link: "#webdev" },
                { name: "Data Analysis", link: "#data" },
                { name: "Digital Marketing", link: "#marketing" },
                { name: "Cloud Services", link: "#cloud" }
            ]
        },
        {
            title: "Resources",
            items: [
                { name: "Documentation", link: "#docs" },
                { name: "Tutorials", link: "#tutorials" },
                { name: "API Reference", link: "#api" }
            ]
        },
        {
            title: "Support",
            items: [
                { name: "FAQs", link: "#faqs" },
                { name: "Contact Us", link: "#contact" },
                { name: "Troubleshooting", link: "#troubleshoot" },
                { name: "Live Chat", link: "#chat" }
            ]
        },
        {
            title: "Company",
            items: [
                { name: "About Us", link: "#about" },
                { name: "Careers", link: "#careers" },
                { name: "Blog", link: "#blog" },
                { name: "Press", link: "#press" },
                { name: "Partners", link: "#partners" }
            ]
        }
    ];

    // Build the menu
    const verticalMenu = document.getElementById('vertical-menu');
    
    menuData.forEach((menuItem, index) => {
        // Create menu item container
        const menuItemElement = document.createElement('div');
        menuItemElement.className = 'menu-item';
        
        // Create menu header
        const menuHeader = document.createElement('div');
        menuHeader.className = 'menu-header';
        menuHeader.innerHTML = `
            ${menuItem.title}
            <span class="arrow">▼</span>
        `;
        
        // Create submenu container
        const submenu = document.createElement('div');
        submenu.className = 'submenu';
        
        // Add submenu items
        menuItem.items.forEach(item => {
            const submenuItem = document.createElement('a');
            submenuItem.className = 'submenu-item';
            submenuItem.href = item.link;
            submenuItem.textContent = item.name;
            submenu.appendChild(submenuItem);
        });
        
        // Append elements to menu item
        menuItemElement.appendChild(menuHeader);
        menuItemElement.appendChild(submenu);
        
        // Append menu item to vertical menu
        verticalMenu.appendChild(menuItemElement);
        
        // Add click event to toggle submenu
        menuHeader.addEventListener('click', function() {
            // Toggle active class
            this.classList.toggle('active');
            submenu.classList.toggle('active');
            
            // Toggle arrow direction
            const arrow = this.querySelector('.arrow');
            if (submenu.classList.contains('active')) {
                arrow.textContent = '▲';
            } else {
                arrow.textContent = '▼';
            }
        });
    });
    
    // Open the first submenu by default
    const firstMenuHeader = document.querySelector('.menu-header');
    const firstSubmenu = document.querySelector('.submenu');
    if (firstMenuHeader && firstSubmenu) {
        firstMenuHeader.classList.add('active');
        firstSubmenu.classList.add('active');
        const arrow = firstMenuHeader.querySelector('.arrow');
        if (arrow) {
            arrow.textContent = '▲';
        }
    }
}); 