# sprintf
An attempt at coding a reduced version of C language sprintf version


```C
printf(":%s:\n", "Hello, world!");
	printf("Hello, world!");
	printf(":%15s:\n", "Hello, world!");
	printf(":%.10s:\n", "Hello, world!");
	printf(":%-10s:\n", "Hello, world!");
	printf(":%-15s:\n", "Hello, world!");
	printf(":%.15s:\n", "Hello, world!");
	printf(":%15.10s:\n", "Hello, world!");
	printf(":%-15.10s:\n", "Hello, world!");
	printf("The color: %s\n", "blue");
	printf("First number: %d\n", 12345);
	printf("Second number: %04d\n", 25);
	printf("Float number: %3.2f\n", 3.14159);
	int Fahrenheit;
	for (Fahrenheit = 0; Fahrenheit <= 300; Fahrenheit = Fahrenheit + 20)
		printf("%3d %06.3f\n", Fahrenheit, (5.0/9.0)*(Fahrenheit-32));
	int a,b;
		float c,d;
		a = 15;
		b = a / 2;
		printf("%d\n",b);
		printf("%3d\n",b);
		printf("%03d\n",b);
		c = 15.3;
		d = c / 3;
		printf("%3.2f\n",d);

```
