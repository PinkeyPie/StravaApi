import React, { useState } from "react";
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'
import { faArrowDown, faArrowRight } from "@fortawesome/free-solid-svg-icons";

const MultiMenus = ({ menus }) => {
    const [activeMenus, setActiveMenus] = useState([]);

    const handleMenuClick = data => {
        console.log(data);
    };

    const handleArrowClick = (menuName) => {
        let newActiveMenus = [...activeMenus];

        if (newActiveMenus.includes(menuName)) {
            var index = newActiveMenus.indexOf(menuName);
            if (index > -1) {
                newActiveMenus.splice(index, 1);
            }
        } else {
            newActiveMenus.push(menuName);
        }

        setActiveMenus(newActiveMenus);
    };

    const ListMenu = ({ dept, data, hasSubMenu, menuName, menuIndex }) => {
        let isOpen = false;
        return (
            <li>
                <div dept={dept}>
                    <p onClick={() => handleMenuClick(data)}>{data.label} </p>
                    {hasSubMenu && (
                        <FontAwesomeIcon icon={faArrowRight} onClick={() => handleArrowClick(menuName)} toggle={activeMenus.includes(menuName)}/>
                    )}
                </div>
                {hasSubMenu && (
                    <SubMenu
                        dept={dept}
                        data={data.submenu}
                        toggle={activeMenus.includes(menuName)}
                        menuIndex={menuIndex}
                    />
                )}
            </li>
        );
    };

    const SubMenu = ({ dept, data, toggle, menuIndex }) => {
        if (!toggle) {
            return null;
        }

        dept = dept + 1;

        return (
            <ul>
                {data.map((menu, index) => {
                    const menuName = `sidebar-submenu-${dept}-${menuIndex}-${index}`;

                    return (
                        <ListMenu
                            dept={dept}
                            data={menu}
                            hasSubMenu={menu.submenu}
                            menuName={menuName}
                            key={menuName}
                            menuIndex={index}
                        />
                    );
                })}
            </ul>
        );
    };

    return (
        <ul>
            {menus.map((menu, index) => {
                const dept = 1;
                const menuName = `sidebar-menu-${dept}-${index}`;

                return (
                    <ListMenu
                        dept={dept}
                        data={menu}
                        hasSubMenu={menu.submenu}
                        menuName={menuName}
                        key={menuName}
                        menuIndex={index}
                    />
                );
            })}
        </ul>
    );
};

export default MultiMenus;



