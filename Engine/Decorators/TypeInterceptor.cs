using System.Reflection;
using System.Reflection.Emit;

namespace Engine.Decorators;

public static class TypeInterceptor
{
    private static readonly Dictionary<Type, Type> InterceptedTypes = new();
    private static readonly ModuleBuilder ModuleBuilder;

    static TypeInterceptor()
    {
        var assemblyName = new AssemblyName("Engine.Decorators.Dynamic");
        var assemblyBuilder = AssemblyBuilder.DefineDynamicAssembly(assemblyName, AssemblyBuilderAccess.Run);
        ModuleBuilder = assemblyBuilder.DefineDynamicModule("DecoratorModule");
    }

    /// <summary>
    /// Создает перехватывающий тип для автоматического применения декораторов
    /// </summary>
    public static Type CreateInterceptedType<T>() where T : class
    {
        return CreateInterceptedType(typeof(T));
    }

    public static Type CreateInterceptedType(Type originalType)
    {
        if (InterceptedTypes.TryGetValue(originalType, out var existingType))
            return existingType;

        if (!HasDecoratedMethods(originalType))
        {
            InterceptedTypes[originalType] = originalType;
            return originalType;
        }

        var typeBuilder = ModuleBuilder.DefineType(
            $"{originalType.Name}_Intercepted",
            TypeAttributes.Public | TypeAttributes.Class,
            originalType);

        // Создаем поле для хранения прокси
        var proxyField = typeBuilder.DefineField("_proxy", originalType, FieldAttributes.Private);

        // Перехватываем конструкторы
        foreach (var constructor in originalType.GetConstructors())
        {
            CreateInterceptedConstructor(typeBuilder, constructor, proxyField, originalType);
        }

        // Перехватываем методы с декораторами
        foreach (var method in originalType.GetMethods(BindingFlags.Public | BindingFlags.Instance))
        {
            if (method.GetCustomAttributes().OfType<IDecoratorAttribute>().Any())
            {
                CreateInterceptedMethod(typeBuilder, method, proxyField);
            }
        }

        var interceptedType = typeBuilder.CreateType();
        InterceptedTypes[originalType] = interceptedType;

        return interceptedType;
    }

    private static void CreateInterceptedConstructor(TypeBuilder typeBuilder, ConstructorInfo originalConstructor,
        FieldBuilder proxyField, Type originalType)
    {
        var parameters = originalConstructor.GetParameters();
        var parameterTypes = parameters.Select(p => p.ParameterType).ToArray();

        var constructorBuilder = typeBuilder.DefineConstructor(
            MethodAttributes.Public,
            CallingConventions.Standard,
            parameterTypes);

        var il = constructorBuilder.GetILGenerator();

        // Вызываем базовый конструктор
        il.Emit(OpCodes.Ldarg_0);
        for (int i = 0; i < parameters.Length; i++)
        {
            il.Emit(OpCodes.Ldarg, i + 1);
        }

        il.Emit(OpCodes.Call, originalConstructor);

        // Создаем прокси: this._proxy = DecoratorDispatchProxy<T>.Create(this);
        il.Emit(OpCodes.Ldarg_0);
        il.Emit(OpCodes.Ldarg_0);

        var createMethod = typeof(DecoratorDispatchProxy<>)
            .MakeGenericType(originalType)
            .GetMethod("Create", BindingFlags.Public | BindingFlags.Static);

        if (createMethod != null) il.Emit(OpCodes.Call, createMethod);
        il.Emit(OpCodes.Stfld, proxyField);

        il.Emit(OpCodes.Ret);
    }

    private static void CreateInterceptedMethod(TypeBuilder typeBuilder, MethodInfo originalMethod,
        FieldBuilder proxyField)
    {
        var parameters = originalMethod.GetParameters();
        var parameterTypes = parameters.Select(p => p.ParameterType).ToArray();

        var methodBuilder = typeBuilder.DefineMethod(
            originalMethod.Name,
            MethodAttributes.Public | MethodAttributes.Virtual,
            originalMethod.ReturnType,
            parameterTypes);

        var il = methodBuilder.GetILGenerator();

        // Вызываем метод через прокси: return this._proxy.MethodName(args);
        il.Emit(OpCodes.Ldarg_0);
        il.Emit(OpCodes.Ldfld, proxyField);

        for (int i = 0; i < parameters.Length; i++)
        {
            il.Emit(OpCodes.Ldarg, i + 1);
        }

        il.Emit(OpCodes.Callvirt, originalMethod);

        if (originalMethod.ReturnType == typeof(void))
        {
            il.Emit(OpCodes.Pop);
        }

        il.Emit(OpCodes.Ret);
    }

    private static bool HasDecoratedMethods(Type type)
    {
        return type.GetMethods(BindingFlags.Public | BindingFlags.Instance)
            .Any(m => m.GetCustomAttributes().OfType<IDecoratorAttribute>().Any());
    }
}