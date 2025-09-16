

struct plat_lcd_data;
struct fb_info;

struct plat_lcd_data {
	int	(*probe)(struct plat_lcd_data *);
	void	(*set_power)(struct plat_lcd_data *, unsigned int power);
	int	(*match_fb)(struct plat_lcd_data *, struct fb_info *);
};

