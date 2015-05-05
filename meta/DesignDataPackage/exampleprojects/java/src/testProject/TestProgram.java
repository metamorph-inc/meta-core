package testProject;
import javax.xml.bind.*;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.StringWriter;
import avm.*;
import modelica.*;
import cad.*;

public class TestProgram {
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		/* Create and marshal a new component */
		try
		{
			System.out.println("hello");
			avm.ObjectFactory of = new avm.ObjectFactory();
			Component c = of.createComponent();
			c.setName("TestComponent");
			
			modelica.ModelicaModel mm = new ModelicaModel();
			mm.setClazz("path.to.class");
			c.getDomainModel().add(mm);
			
			StringWriter writer = new StringWriter();
			JAXBContext context = JAXBContext.newInstance(Component.class);
			Marshaller m = context.createMarshaller();
		    m.setProperty( Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE );
			m.marshal(c, writer);
			
			System.out.println(writer.toString());
		}
		catch (JAXBException jex)
		{
			System.out.println(jex.getMessage());		
		}
		
		/* Unmarshal the XML file from the Python version */
		try {
			System.out.println("--------------------------------");
			System.out.println("Unmarshalled from Python example");
			System.out.println("--------------------------------");
						
			FileInputStream fis = new FileInputStream("../python/python_test_out.acm");
			JAXBContext context = JAXBContext.newInstance(Component.class);
			Unmarshaller um = context.createUnmarshaller();
			Component c = (Component)um.unmarshal(fis);
			
			System.out.println("Component: " + c.getName());
			
			for (DomainModel dm : c.getDomainModel())
			{
				System.out.println("\nDomain Model (" + dm.getClass().getName() + ")");
				if (dm.getClass() == CADModel.class)
				{
					CADModel cm = (CADModel)dm;
					System.out.println("Notes: " + cm.getNotes());
				} 
				else if (dm.getClass() == ModelicaModel.class)
				{
					ModelicaModel mm = (ModelicaModel)dm;
					System.out.println("Class: " + mm.getClazz());					
				}
			}
			
			/* Serialize it right back out */
			try {
				FileOutputStream fos;
				fos = new FileOutputStream("java_test_out.acm");
				Marshaller m = context.createMarshaller();
			    m.setProperty( Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE );
				m.marshal(c,fos);
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (JAXBException jes) {
			jes.printStackTrace();
		}
	}
}
